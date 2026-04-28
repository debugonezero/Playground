import http.server
import socketserver
import json
import os
import subprocess
from pathlib import Path

PORT = 8080
SESSION_DIR = Path("sessions") # Ignored by Git (Logs, Telemetry)
RESEARCH_DIR = Path("research") # Tracked by Git (Code, Payloads)

SESSION_DIR.mkdir(exist_ok=True)
RESEARCH_DIR.mkdir(exist_ok=True)

class AxiomaticRelayHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        parts = self.path.strip("/").split("/")
        if len(parts) == 3 and parts[0] == "relay":
            session_id, data_type = parts[1], parts[2]
            
            # --- THE ARCHITECTURAL SPLIT ---
            # 1. Payloads come from the Git-tracked 'research' folder
            if data_type == "payload":
                payload_path = RESEARCH_DIR / session_id / "payload.json"
                if payload_path.exists():
                    self.send_response(200)
                    self.send_header("Content-Type", "application/json")
                    self.send_header("Access-Control-Allow-Origin", "*")
                    self.end_headers()
                    with open(payload_path, "rb") as f:
                        self.wfile.write(f.read())
                else:
                    self.send_error(404, "Payload not found in research branch.")
                return
            
            # 2. Output and Specs come from the ignored 'sessions' folder
            session_path = SESSION_DIR / session_id
            file_path = session_path / f"{data_type}.json"
            if file_path.exists():
                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.send_header("Access-Control-Allow-Origin", "*")
                self.end_headers()
                with open(file_path, "rb") as f:
                    self.wfile.write(f.read())
            else:
                self.send_error(404, "Telemetry not found")
            return
            
        return super().do_GET()

    def do_POST(self):
        parts = self.path.strip("/").split("/")
        if len(parts) >= 3 and parts[0] == "relay":
            session_id, data_type = parts[1], parts[2]
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            
            if data_type == "execute_mps":
                print(f">>> EXECUTING NATIVE MPS FOR {session_id}...")
                try:
                    res = subprocess.check_output(["swift", "/Volumes/AI_Vault/debugonezero/Playground/mps_research.swift"])
                    self.send_response(200)
                    self.send_header("Access-Control-Allow-Origin", "*")
                    self.end_headers()
                    self.wfile.write(res)
                except Exception as e:
                    self.send_error(500, str(e))
                return
            
            # Save telemetry data (output.json, hardware_specs.json) to ignored folder
            session_path = SESSION_DIR / session_id
            session_path.mkdir(parents=True, exist_ok=True)
            
            file_path = session_path / f"{data_type}.json"
            with open(file_path, "wb") as f:
                f.write(post_data)
            
            self.send_response(200)
            self.send_header("Access-Control-Allow-Origin", "*")
            self.end_headers()
            self.wfile.write(b'{"status": "success"}')
            return

    def do_OPTIONS(self):
        self.send_response(200)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()

print(f"Axiomatic Archival Server active at http://localhost:{PORT}")
with socketserver.TCPServer(("", PORT), AxiomaticRelayHandler) as httpd:
    httpd.serve_forever()

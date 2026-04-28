import http.server
import socketserver
import json
import os
from pathlib import Path

PORT = 8080
SESSION_DIR = Path("sessions")
SESSION_DIR.mkdir(exist_ok=True)

class AxiomaticRelayHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        # Route: /relay/{session_id}/payload
        # Route: /relay/{session_id}/output
        parts = self.path.strip("/").split("/")
        if len(parts) == 3 and parts[0] == "relay":
            session_id, data_type = parts[1], parts[2]
            file_path = SESSION_DIR / session_id / f"{data_type}.json"
            
            if file_path.exists():
                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.send_header("Access-Control-Allow-Origin", "*")
                self.end_headers()
                with open(file_path, "rb") as f:
                    self.wfile.write(f.read())
            else:
                self.send_error(404, "File not found")
            return
            
        return super().do_GET()

    def do_POST(self):
        # Route: /relay/{session_id}/{data_type}
        parts = self.path.strip("/").split("/")
        if len(parts) == 3 and parts[0] == "relay":
            session_id, data_type = parts[1], parts[2]
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            
            # Ensure directory exists
            session_path = SESSION_DIR / session_id
            session_path.mkdir(exist_ok=True)
            
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

print(f"Axiomatic Relay Server active at http://localhost:{PORT}")
with socketserver.TCPServer(("", PORT), AxiomaticRelayHandler) as httpd:
    httpd.serve_forever()

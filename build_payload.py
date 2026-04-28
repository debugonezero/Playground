import json
import os

html = """
<div style="position:absolute; inset:0; background:#020508; color:#00ff9d; font-family:'Inter', sans-serif; display:flex; flex-direction:column; justify-content:center; align-items:center;">
    <h1 style="font-weight:900; letter-spacing:10px; margin:0;">ARCHIVAL_PROTOCOL_ACTIVE</h1>
    <div style="margin-top:20px; font-size:14px; opacity:0.7; font-family:monospace;">
        This payload was automatically fetched from 'payload_001_intro.json'.<br>
        Your previous work is safely isolated in its own session folder.
    </div>
</div>
"""
js = "window.axiomaticLog({ event: 'Archival_Test_Success', version: 1 });"

payload = { 'id': 'archival_v1', 'html': html, 'js': js }

# Create a brand new session folder to test isolation
os.makedirs('Playground/sessions/NEW_TEST_1', exist_ok=True)
with open('Playground/sessions/NEW_TEST_1/payload_001_intro.json', 'w') as f:
    json.dump(payload, f)
print("Archival Payload deployed.")

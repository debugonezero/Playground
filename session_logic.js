<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>Axiomatic Playground</title>
    <style>
        body { margin: 0; background: #000; color: #00ff9d; font-family: monospace; overflow: hidden; display: flex; flex-direction: column; height: 100vh; }
        #canvas-container { flex-grow: 1; background: #000; position: relative; }
        #terminal { height: 30%; background: #0a0a0a; border-top: 1px solid #333; display: flex; flex-direction: column; padding: 10px; box-sizing: border-box; }
        #logs { flex-grow: 1; overflow-y: auto; font-size: 11px; margin-bottom: 10px; }
        .log-entry { border-left: 2px solid #00ff9d; padding-left: 8px; margin-bottom: 2px; }
        .controls { display: flex; gap: 10px; }
        button { background: #00ff9d; color: #000; border: none; padding: 10px 20px; font-weight: bold; cursor: pointer; font-family: monospace; }
        button:active { background: #008f58; }
    </style>
</head>
<body>

    <div id="canvas-container"></div>

    <div id="terminal">
        <div id="logs"></div>
        <div class="controls">
            <button id="pasteBtn">PASTE & EXECUTE</button>
        </div>
    </div>

    <script>
        const logContainer = document.getElementById('logs');
        
        window.axiomaticLog = (data) => {
            const entry = document.createElement('div');
            entry.className = 'log-entry';
            entry.innerText = `[${new Date().toLocaleTimeString()}] ${data}`;
            logContainer.appendChild(entry);
            logContainer.scrollTop = logContainer.scrollHeight;
        };

        window.inject = async (payload) => {
            try {
                window.axiomaticLog("Injecting: " + payload.id);
                document.getElementById('canvas-container').innerHTML = payload.html || "";
                const run = new Function('payload', payload.js);
                await run(payload);
                window.axiomaticLog("Status: Online");
            } catch (err) {
                window.axiomaticLog("Error: " + err.message);
            }
        };

        document.getElementById('pasteBtn').addEventListener('click', async () => {
            try {
                const text = await navigator.clipboard.readText();
                // This executes the inject({...}) code inside the clipboard
                eval(text);
            } catch (err) {
                window.axiomaticLog("Clipboard Error: " + err.message);
            }
        });

        window.axiomaticLog("Axiomatic Ready.");
    </script>
</body>
</html>

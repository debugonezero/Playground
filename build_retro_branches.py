import os
import subprocess

# Define the previous experiments to recreate
branches = {
    "ZEN-001": {
        "title": "Cosmic Drifter (Zen Game)",
        "desc": "Fluid dynamics and particle system demonstration."
    },
    "BENCH-001": {
        "title": "WebGPU / Metal Tensor Benchmark",
        "desc": "Matrix Multiplication stress test hitting 11+ GFLOPS."
    },
    "ONC-KMN": {
        "title": "Oncology K-Means (Breast Cancer)",
        "desc": "Unsupervised classification of cytology tensors."
    },
    "ONC-GNN": {
        "title": "Oncology Graph Neural Network",
        "desc": "Message passing simulation on Patient k-NN graph."
    }
}

os.chdir("/Volumes/AI_Vault/debugonezero/Playground")

# Ensure main is clean
subprocess.run(["git", "add", "."])
subprocess.run(["git", "commit", "-m", "chore: setup relay server architecture"])

for session_id, data in branches.items():
    print(f"Building Branch: session/{session_id}")
    
    # 1. Create and switch to new branch
    subprocess.run(["git", "checkout", "-b", f"session/{session_id}"])
    
    # 2. Setup the directory structure
    research_dir = f"research/{session_id}"
    os.makedirs(research_dir, exist_ok=True)
    
    # 3. Create Manifest
    with open(f"{research_dir}/manifest.md", "w") as f:
        f.write(f"# Session: {session_id}\n")
        f.write(f"**Focus:** {data['title']}\n")
        f.write(f"**Details:** {data['desc']}\n")
    
    # 4. Generate dummy payload (We can inject the real ones later if needed, 
    # but this establishes the Git tree immediately).
    payload = {
        "id": session_id,
        "html": f"<div style='padding:50px; color:#00ff9d; font-family:monospace; background:#000; height:100%;'><h1>{data['title']}</h1><p>Restored via Axiomatic Branching.</p></div>",
        "js": f"window.axiomaticLog({{ event: 'Restored', session: '{session_id}' }});"
    }
    with open(f"{research_dir}/payload.json", "w") as f:
        json.dump(payload, f)
        
    # 5. Commit
    subprocess.run(["git", "add", "research/"])
    subprocess.run(["git", "commit", "-m", f"feat: save payload for {session_id}"])
    
    # 6. Go back to main
    subprocess.run(["git", "checkout", "main"])

print("Retroactive Branching Complete!")

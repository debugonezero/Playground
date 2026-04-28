#!/bin/bash

COMMAND=$1
SESSION_ID=$2

if [ "$COMMAND" == "start" ]; then
    if [ -z "$SESSION_ID" ]; then
        echo "Usage: ./axiom.sh start <SESSION_ID>"
        exit 1
    fi
    
    echo ">>> INITIATING AXIOMATIC BRANCH: $SESSION_ID"
    
    # Ensure we are on main before branching
    git checkout main -q
    
    # Create and checkout the new branch
    git checkout -b "session/$SESSION_ID" 2>/dev/null || git checkout "session/$SESSION_ID"
    
    # Create the research directory for the payload
    mkdir -p "research/$SESSION_ID"
    
    # Create a manifest
    if [ ! -f "research/$SESSION_ID/manifest.md" ]; then
        echo "# Session: $SESSION_ID" > "research/$SESSION_ID/manifest.md"
        echo "**Date:** $(date)" >> "research/$SESSION_ID/manifest.md"
        echo "**Status:** ACTIVE" >> "research/$SESSION_ID/manifest.md"
    fi
    
    echo ">>> SWITCHED TO BRANCH: session/$SESSION_ID"
    echo ">>> WORKSPACE READY. Agent will deploy code to research/$SESSION_ID/payload.json"

elif [ "$COMMAND" == "save" ]; then
    # Commit current branch
    git add research/
    git commit -m "save: update research payloads"
    echo ">>> BRANCH SAVED."
else
    echo "Axiomatic CLI"
    echo "Commands:"
    echo "  start <ID>  - Create/switch to a session branch"
    echo "  save        - Commit the current session progress"
fi

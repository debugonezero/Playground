/**
 * AXIOMATIC SESSION GENERATOR
 * Run this to create a shareable URL for the agent.
 */
function createSharedSession() {
    const sessionToken = Math.random().toString(36).substring(2, 8).toUpperCase();
    const currentUrl = window.location.origin + window.location.pathname;
    const shareableLink = `${currentUrl}?session=${sessionToken}`;
    
    console.log("--- NEW SESSION GENERATED ---");
    console.log("Session ID:", sessionToken);
    console.log("Share this URL with the Agent:", shareableLink);
    
    // If running on your phone, you can copy this to clipboard
    return shareableLink;
}

// Example usage: Call createSharedSession() from your console or a button.

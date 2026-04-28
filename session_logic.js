/**
 * AXIOMATIC SESSION GENERATOR
 * Handles session creation and clipboard interaction.
 */

function createSharedSession() {
    const sessionToken = Math.random().toString(36).substring(2, 8).toUpperCase();
    const currentUrl = window.location.origin + window.location.pathname;
    const shareableLink = `${currentUrl}?session=${sessionToken}`;
    
    console.log("--- NEW SESSION GENERATED ---");
    console.log("Session ID:", sessionToken);
    console.log("Shareable Link:", shareableLink);
    
    return shareableLink;
}

/**
 * Copies the generated session URL to the clipboard and logs the event.
 */
async function copySharedSession() {
    const url = createSharedSession();
    try {
        await navigator.clipboard.writeText(url);
        if (window.axiomaticLog) {
            window.axiomaticLog({ 
                event: "SESSION_COPIED", 
                message: "Shareable URL copied to clipboard!",
                url: url 
            });
        }
        alert("Session URL copied to clipboard!\n" + url);
    } catch (err) {
        console.error('Failed to copy: ', err);
        alert("Failed to copy to clipboard. Check console for URL.");
    }
}

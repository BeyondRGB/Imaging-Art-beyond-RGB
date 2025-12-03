/**
 * CSS Utility functions for accessing CSS custom properties (variables)
 */

/**
 * Gets a CSS custom property value from the document root.
 * Returns a fallback value if running in a non-browser environment (SSR).
 * 
 * @param name - The CSS variable name (e.g., '--color-text-primary')
 * @param fallback - Optional fallback value if the variable is not found
 * @returns The trimmed value of the CSS variable, or the fallback
 */
export function getCssVar(name: string, fallback: string = '#ffffff'): string {
    if (typeof window !== 'undefined') {
        const value = getComputedStyle(document.documentElement).getPropertyValue(name).trim();
        return value || fallback;
    }
    return fallback;
}


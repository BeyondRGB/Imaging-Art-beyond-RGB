describe('About Modal', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
    // Wait for app to load - the Home modal appears first
    cy.wait(1000);
    // Home content is visible inside the modal - don't close it, interact with it directly
    // Wait for home content to be visible (it's inside the modal)
    cy.get("#homeContent", { timeout: 10000 }).should("be.visible");
  });

  it('Opens About modal from home page', () => {
    cy.get("#homeContent .homeBtn", { timeout: 10000 }).contains("About").click();
    cy.wait(500); // Wait for modal animation
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
    cy.get(".aboutBox", { timeout: 5000 }).should("be.visible");
  });

  it('About modal contains expected content', () => {
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.wait(500); // Wait for modal animation
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
    // Check for project information - About component should contain "Beyond"
    cy.get(".aboutBox", { timeout: 5000 }).should("be.visible").should("contain.text", "Beyond");
  });

  it('About modal can be closed', () => {
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.wait(500); // Wait for modal animation
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
    
    // Find and click close button
    cy.get(".aboutContent .closeBtn", { timeout: 5000 }).should("be.visible").click();
    cy.wait(500); // Wait for close animation
    cy.get(".aboutContent").should("not.exist");
  });

  it('Can open About from multiple locations', () => {
    // From home modal
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.wait(500);
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
    cy.get(".aboutContent .closeBtn", { timeout: 5000 }).should("be.visible").click();
    cy.wait(500);
    cy.get(".aboutContent").should("not.exist");
    
    // Navigate to Process
    cy.get("#homeContent .homeBtn").contains("Process").click();
    cy.wait(500);
    
    // Navigate back to home and try About again
    cy.get(".ctlBtns button").first().click(); // Home button - opens Home modal
    cy.wait(500);
    cy.get("#homeContent", { timeout: 5000 }).should("be.visible");
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.wait(500);
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
  });
});


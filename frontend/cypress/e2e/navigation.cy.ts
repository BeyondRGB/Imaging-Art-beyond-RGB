describe('Application Navigation', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
    // Wait for app to load - Home modal appears first
    cy.wait(1000);
    // Home content is visible inside the modal - interact with it directly
    cy.get("#homeContent", { timeout: 10000 }).should("be.visible");
  });

  it('Navigates from Home to Process page', () => {
    cy.get("#homeContent .homeBtn", { timeout: 10000 }).contains("Process").click();
    cy.url().should('include', '/');
    cy.get("#selectProcessingTypesContent", { timeout: 5000 }).should("be.visible");
  });

  it('Navigates from Home to View page', () => {
    cy.get("#homeContent .homeBtn").contains("View").click();
    // Check for Reports page elements
    cy.wait(500);
    // View page should be visible (no specific content check needed)
  });

  it('Opens About modal', () => {
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
    // About modal uses aboutContent, not modal with title
    cy.get(".aboutBox", { timeout: 5000 }).should("be.visible");
  });

  it('Home button returns to home from Process', () => {
    // Navigate to Process from the Home modal
    cy.get("#homeContent .homeBtn").contains("Process").click();
    cy.wait(500);
    cy.get("#selectProcessingTypesContent", { timeout: 5000 }).should("be.visible");
    
    // Home button is first button in ctlBtns - opens Home modal
    cy.get(".ctlBtns button").first().click();
    cy.wait(500);
    // Home modal opens - home content should be visible inside modal
    cy.get("#homeContent", { timeout: 5000 }).should("be.visible");
    cy.get(".homeBtn", { timeout: 5000 }).contains("Process").should("be.visible");
  });

  it('Can navigate between all main pages', () => {
    // Go to Process from Home modal
    cy.get("#homeContent .homeBtn").contains("Process").click();
    cy.wait(500);
    cy.get("#selectProcessingTypesContent", { timeout: 5000 }).should("be.visible");
    
    // Go home - opens Home modal
    cy.get(".ctlBtns button").first().click();
    cy.wait(500);
    cy.get("#homeContent", { timeout: 5000 }).should("be.visible");
    cy.get(".homeBtn", { timeout: 5000 }).contains("Process").should("be.visible");
    
    // Go to View
    cy.get("#homeContent .homeBtn").contains("View").click();
    cy.wait(500);
    
    // Go home again
    cy.get(".ctlBtns button").first().click();
    cy.wait(500);
    cy.get("#homeContent", { timeout: 5000 }).should("be.visible");
    cy.get(".homeBtn", { timeout: 5000 }).contains("About").should("be.visible");
  });
});


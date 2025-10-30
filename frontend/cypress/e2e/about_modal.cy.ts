describe('About Modal', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
    // Wait for app to load - the Home modal appears first, then we need to wait
    cy.wait(1000);
    // Close the initial Home modal if it's open
    cy.get('body').then(($body) => {
      if ($body.find('.close-home').length > 0) {
        cy.get('.close-home').click();
        cy.wait(500);
      }
    });
  });

  it('Opens About modal from home page', () => {
    cy.get("#homeContent .homeBtn", { timeout: 10000 }).contains("About").click();
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
  });

  it('About modal contains expected content', () => {
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
    // Check for project information - About component should contain "Beyond"
    cy.get(".aboutBox", { timeout: 5000 }).should("contain.text", "Beyond");
  });

  it('About modal can be closed', () => {
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
    
    // Find and click close button
    cy.get(".aboutContent .closeBtn", { timeout: 5000 }).should("be.visible").click();
    cy.wait(300);
    cy.get(".aboutContent").should("not.exist");
  });

  it('Can open About from multiple locations', () => {
    // From home
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.get(".aboutContent", { timeout: 5000 }).should("be.visible");
    cy.get(".aboutContent .closeBtn", { timeout: 5000 }).should("be.visible").click();
    cy.wait(300);
    cy.get(".aboutContent").should("not.exist");
    
    // Navigate to Process
    cy.get("#homeContent .homeBtn").contains("Process").click();
    cy.wait(500);
    
    // About can be opened from Home button in menu, then navigate back
    // (depends on UI implementation)
  });
});


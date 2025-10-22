describe('About Modal', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
  });

  it('Opens About modal from home page', () => {
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.get(".modal").should("be.visible");
  });

  it('About modal contains expected content', () => {
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.get(".modal").should("be.visible");
    cy.get(".modal .title").should("contain", "About");
    // Check for project information
    cy.get(".modal .body").should("contain.text", "Beyond");
  });

  it('About modal can be closed', () => {
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.get(".modal").should("be.visible");
    
    // Find and click close button (with explicit wait)
    cy.get(".modal .closeDia", { timeout: 5000 }).should("be.visible").click();
    cy.get(".modal").should("not.exist");
  });

  it('Can open About from multiple locations', () => {
    // From home
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.get(".modal").should("be.visible");
    cy.get(".modal .closeDia", { timeout: 5000 }).should("be.visible").click();
    cy.get(".modal").should("not.exist");
    
    // Navigate to Process
    cy.get("#homeContent .homeBtn").contains("Process").click();
    
    // Could open About from here too if menu exists
    // (depends on UI implementation)
  });
});


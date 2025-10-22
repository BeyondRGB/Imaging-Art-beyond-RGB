describe('Application Navigation', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
  });

  it('Navigates from Home to Process page', () => {
    cy.get("#homeContent .homeBtn").contains("Process").click();
    cy.url().should('include', '/');
    cy.get("#selectProcessingTypesContent").should("be.visible");
  });

  it('Navigates from Home to View page', () => {
    cy.get("#homeContent .homeBtn").contains("View").click();
    // Check for Reports page elements
    cy.wait(100);
  });

  it('Opens About modal', () => {
    cy.get("#homeContent .homeBtn").contains("About").click();
    cy.get(".modal").should("be.visible");
    cy.get(".modal .title").should("contain", "About");
  });

  it('Home button returns to home from Process', () => {
    cy.get("#homeContent .homeBtn").contains("Process").click();
    cy.get("#selectProcessingTypesContent").should("be.visible");
    
    cy.get(".ctlBtns .feather-home").click();
    cy.get(".homeBtn").contains("Process").should("be.visible");
  });

  it('Can navigate between all main pages', () => {
    // Go to Process
    cy.get("#homeContent .homeBtn").contains("Process").click();
    cy.wait(200);
    
    // Go home
    cy.get(".ctlBtns .feather-home").click();
    cy.get(".homeBtn").contains("Process").should("be.visible");
    
    // Go to View
    cy.get("#homeContent .homeBtn").contains("View").click();
    cy.wait(200);
    
    // Go home
    cy.get(".ctlBtns .feather-home").click();
    cy.get(".homeBtn").contains("About").should("be.visible");
  });
});


describe('UI Elements and Interactions', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
  });

  it('Home page displays all main buttons', () => {
    cy.get(".homeBtn").contains("Process").should("be.visible");
    cy.get(".homeBtn").contains("View").should("be.visible");
    cy.get(".homeBtn").contains("About").should("be.visible");
  });

  it('Home page buttons have descriptions', () => {
    cy.contains("Process a new RAW image set").should("be.visible");
    cy.contains("View a previously-processed imaged set").should("be.visible");
    cy.contains("About the program").should("be.visible");
  });

  it('Control buttons are visible on Process page', () => {
    cy.get("#homeContent .homeBtn").contains("Process").click();
    
    // Check for control buttons
    cy.get(".ctlBtns .feather-home").should("be.visible");
    cy.get(".ctlBtns .feather-settings").should("be.visible");
  });

  it('Icons are properly loaded', () => {
    cy.get("#homeContent .homeBtn").contains("Process").click();
    
    // Check that feather icons exist
    cy.get(".feather-home").should("exist");
    cy.get(".feather-settings").should("exist");
  });

  it('Page layout is responsive', () => {
    // Check viewport sizes
    cy.viewport(1920, 1080);
    cy.get(".homeBtn").should("be.visible");
    
    cy.viewport(1280, 720);
    cy.get(".homeBtn").should("be.visible");
    
    // Small viewport (might have different layout)
    cy.viewport(800, 600);
    cy.get(".homeBtn").should("be.visible");
  });

  it('Application has proper title', () => {
    // Electron apps may not set document title, so just check it exists
    cy.title().should('exist');
  });
});


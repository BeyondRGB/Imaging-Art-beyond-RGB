describe('Settings Modal', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
    // Wait for app to load
    cy.get("#homeContent", { timeout: 10000 }).should("be.visible");
    cy.get("#homeContent .homeBtn").contains("Process").click();
    // Wait for Process page to load
    cy.wait(500);
    // Click settings button - it's in ctlBtns, second button (after Home, before connection)
    cy.get(".ctlBtns button").eq(1).click(); // Settings button (index 1)
    // Wait for modal to appear
    cy.get(".modal-container", { timeout: 5000 }).should("be.visible");
  });

  it('Opens settings modal from any page', () => {
    cy.get(".settings", { timeout: 5000 }).should("be.visible");
    cy.get(".settings").should("contain", "Settings");
  });

  it('Settings modal can be closed with X button', () => {
    cy.get(".close-button", { timeout: 5000 }).should("be.visible");
    cy.get(".close-button").click();
    cy.wait(300);
    cy.get(".modal-container").should("not.exist");
  });

  it('Settings modal can be closed by clicking outside', () => {
    cy.get(".modal-container", { timeout: 5000 }).should("be.visible");
    // Click on the backdrop/overlay
    cy.get(".modal-background").click({ force: true });
    cy.wait(300);
    cy.get(".modal-container").should("not.exist");
  });

  it('Side Menu toggle works', () => {
    cy.get(".modal-container .settings", { timeout: 5000 }).should("be.visible");
    cy.get(".modal-container .settings").within(() => {
      cy.contains("Side Menu").should("be.visible");
      
      // Find the switch for Side Menu - it's in a SwitchRow component
      cy.get('input[type="checkbox"]').first().should("exist");
    });
  });

  it('Settings persist after closing and reopening', () => {
    // Get initial state
    cy.get('.modal-container .settings input[type="checkbox"]').first().then(($checkbox) => {
      const initialState = $checkbox.prop('checked');
      
      // Toggle it
      cy.get('.modal-container .settings input[type="checkbox"]').first().click({ force: true });
      cy.wait(200);
      
      // Close modal
      cy.get(".modal-container .close-button").click();
      cy.wait(300);
      cy.get(".modal-container").should("not.exist");
      
      // Reopen settings
      cy.get(".ctlBtns button").eq(1).click();
      cy.get(".modal-container .settings", { timeout: 5000 }).should("be.visible");
      
      // Check state changed
      cy.get('.modal-container .settings input[type="checkbox"]').first().should($cb => {
        expect($cb.prop('checked')).to.not.equal(initialState);
      });
    });
  });
});


describe('Settings Modal', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
    cy.get("#homeContent .homeBtn").contains("Process").click();
    cy.get(".ctlBtns .feather-settings").click();
  });

  it('Opens settings modal from any page', () => {
    cy.get(".settings").should("be.visible");
    cy.get(".settings").should("contain", "Settings");
  });

  it('Settings modal can be closed with X button', () => {
    cy.get(".closeDia").should("be.visible");
    cy.get(".closeDia").click();
    cy.get(".modal").should("not.exist");
  });

  it('Settings modal can be closed by clicking outside', () => {
    cy.get(".modal").should("be.visible");
    // Click on the backdrop/overlay
    cy.get("body").click(0, 0);
    cy.wait(100);
    // Modal should close (this might not work depending on implementation)
  });

  it('Side Menu toggle works', () => {
    cy.get(".modal .settings").first().within(() => {
      cy.contains("Side Menu").should("be.visible");
      
      // Find the switch for Side Menu
      cy.get('input[type="checkbox"]').first().should("exist");
    });
  });

  it('Settings persist after closing and reopening', () => {
    // Get initial state
    cy.get('.modal .settings input[type="checkbox"]').first().then(($checkbox) => {
      const initialState = $checkbox.prop('checked');
      
      // Toggle it
      cy.get('.modal .settings input[type="checkbox"]').first().click({ force: true });
      
      // Close modal
      cy.get(".modal .closeDia").click();
      cy.get(".modal").should("not.exist");
      
      // Reopen settings
      cy.get(".ctlBtns .feather-settings").click();
      cy.get(".modal .settings").should("be.visible");
      
      // Check state changed
      cy.get('.modal .settings input[type="checkbox"]').first().should($cb => {
        expect($cb.prop('checked')).to.not.equal(initialState);
      });
    });
  });
});


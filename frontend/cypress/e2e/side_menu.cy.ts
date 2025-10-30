describe('Side Menu', () => {
    beforeEach(() => {
      cy.visit('http://localhost:3000/');
      // Wait for app to load - Home modal appears first
      cy.wait(1000);
      // Home content is visible inside the modal
      cy.get("#homeContent", { timeout: 10000 }).should("be.visible");
    })

    it('Settings button works', () => {
      cy.get("#homeContent .homeBtn", { timeout: 10000 }).contains("Process").click();
      cy.wait(500);
      // Settings button is second button in ctlBtns (index 1)
      cy.get(".ctlBtns button").eq(1).click();
      cy.get(".modal-container .settings", { timeout: 5000 }).should("contain", "Settings");
      cy.get(".modal-container .close-button", { timeout: 5000 }).click();
      cy.wait(300);
      cy.get(".modal-container").should("not.exist");
    })
    
    it('Home button works', () => {
      // Close initial Home modal first
      cy.get('.close-home', { timeout: 5000 }).click();
      cy.wait(500);
      
      // Navigate to Process by opening Home modal again
      cy.get(".ctlBtns button").first().click(); // Open Home modal
      cy.wait(500);
      cy.get("#homeContent", { timeout: 5000 }).should("be.visible");
      cy.get("#homeContent .homeBtn").contains("Process").click();
      cy.wait(500);
      
      // Home button is first button in ctlBtns - opens Home modal
      cy.get(".ctlBtns button").first().click();
      cy.wait(500);
      // Home modal opens - home content should be visible inside modal
      cy.get("#homeContent", { timeout: 5000 }).should("be.visible");
      cy.get(".homeBtn", { timeout: 5000 }).contains("Process").should("be.visible");
    })
  })
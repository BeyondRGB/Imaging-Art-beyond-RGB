describe('Side Menu', () => {
    beforeEach(() => {
      cy.visit('http://localhost:3000/');
    })

    it('Settings button works', () => {
      cy.get(".homeBtn").contains("Process").click();
      cy.get(".ctlBtns .feather-settings").click();
      cy.get(".settings").should("contain", "Settings");
      cy.get(".closeDia").click();
      cy.get(".modal").should("not.exist");
    })
    it('Home button works', () => {
      cy.get(".homeBtn").contains("Process").click();
      cy.get(".ctlBtns .feather-home").click();
      cy.get(".homeBtn").contains("Process").should("be.visible");
    })
  })
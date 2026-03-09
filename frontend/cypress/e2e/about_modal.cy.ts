describe("About Modal", () => {
	beforeEach(() => {
		cy.clearLocalStorage();
		cy.visit("/");
	});

	it("opens and closes the about dialog from the home screen", () => {
		cy.contains("#homeContent button", "About").click();

		cy.contains("About Beyond RGB").should("be.visible");
		cy.contains("Senior Project Team 5").should("exist");
		cy.contains("Jakob Langtry").scrollIntoView().should("be.visible");

		cy.get(".close-button").click();
		cy.contains("About Beyond RGB").should("not.exist");
	});
});

describe("Side Menu", () => {
	beforeEach(() => {
		cy.clearLocalStorage();
		cy.visit("/");
	});

	it("Settings button works", () => {
		cy.contains("#homeContent button", "Process").click();
		cy.get("#homeContent").should("not.exist");
		cy.get('[data-cy="nav-settings"]').click();
		cy.get('[role="dialog"]').should("exist");
		cy.get(".close-button").should("be.visible");
		cy.get(".close-button").click();
		cy.get(".close-button").should("not.exist");
	});
	it("Home button works", () => {
		cy.contains("#homeContent button", "Process").click();
		cy.get('[data-cy="nav-home"]').click();
		cy.contains("#homeContent button", "Process").should("be.visible");
	});
});

describe("Home Page", () => {
	it("Contains the correct elements", () => {
		cy.clearLocalStorage();
		cy.visit("/");

		cy.contains("#homeContent button", "Process");
		cy.contains("#homeContent", "Process a new RAW image set");
		cy.contains("#homeContent button", "View");
		cy.contains("#homeContent", "View a previously-processed imaged set");
		cy.contains("#homeContent button", "Create Another Window");
		cy.contains("#homeContent button", "About");
		cy.contains("#homeContent", "About the program");
	});
});

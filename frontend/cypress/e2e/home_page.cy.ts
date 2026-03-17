describe("Home Page", () => {
	it("Contains the correct elements", () => {
		cy.visit("http://localhost:3000/");

		cy.contains("Process");
		cy.contains("Process a new RAW image set");
		cy.contains("View");
		cy.contains("View a previously-processed imaged set");
		cy.contains("About");
		cy.contains("About the program");

		cy.contains("About the program").click();
		cy.contains("About Beyond RGB");
		cy.contains("Version 2.4.0");
	});
});

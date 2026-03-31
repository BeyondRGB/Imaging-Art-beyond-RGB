describe("Home Page", () => {
	it("Contains the correct elements", () => {
		cy.clearLocalStorage();
		cy.visit("/");

		cy.contains("#homeContent button", "Process").should("be.visible");
		cy.contains("#homeContent", "Process a new RAW image set").should("be.visible");
		cy.contains("#homeContent button", "View").should("be.visible");
		cy.contains("#homeContent", "View a previously-processed imaged set").should("be.visible");
		cy.contains("#homeContent button", "Create Another Window").should("be.visible");
		cy.contains("#homeContent button", "About").should("be.visible");
		cy.contains("#homeContent", "About the program").should("be.visible");
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

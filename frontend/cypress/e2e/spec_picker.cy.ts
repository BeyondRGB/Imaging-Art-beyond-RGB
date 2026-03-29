describe("Spectral Picker Landing State", () => {
	it("shows the empty state when opening the viewer from home", () => {
		cy.clearLocalStorage();
		cy.visit("/");
		cy.contains("#homeContent button", "View").click();

		cy.contains("Select a project file to import into BeyondRGB").should("be.visible");
		cy.contains("button", "Select Files").should("be.visible");
		cy.contains("Accepted: BeyondRGB Project (.btrgb)").should("be.visible");
		cy.contains("or drag and drop files here").should("be.visible");
	});
});

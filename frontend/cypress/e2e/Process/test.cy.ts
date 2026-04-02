describe("Select Processing Type and Import Images Tab", () => {
	beforeEach(() => {
		cy.clearLocalStorage();
		cy.visit("/");
		cy.contains("#homeContent button", "Process").click();
	});

	it("Batch Processing button works", () => {
		cy.contains("#selectProcessingTypesContent button", "Batch Processing").click();

		cy.contains("button", "Back").should("be.visible");
		cy.contains(".custom-dropzone", "Drag and Drop Files Here").should("be.visible");
		cy.contains("h1", "Import Images").should("be.visible");
		cy.contains("button", "Back").click();
		cy.contains("#selectProcessingTypesContent button", "Batch Processing").should("be.visible");
	});
	it("Single Image Processing button works", () => {
		cy.contains("#selectProcessingTypesContent button", "Single Image Processing").click();

		cy.contains("button", "Back").should("be.visible");
		cy.contains(".custom-dropzone", "Drag and Drop Files Here").should("be.visible");
		cy.contains("h1", "Import Images").should("be.visible");
		cy.contains("button", "Back").click();
		cy.contains("#selectProcessingTypesContent button", "Single Image Processing").should(
			"be.visible"
		);
	});
});

describe('Process Workflow', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
    // Wait for app to load - Home modal appears first
    cy.wait(1000);
    // Home content is visible inside the modal
    cy.get("#homeContent", { timeout: 10000 }).should("be.visible");
    cy.get("#homeContent .homeBtn", { timeout: 10000 }).contains("Process").click();
    cy.wait(500);
  });

  it('Shows processing type selection screen', () => {
    cy.get("#selectProcessingTypesContent").should("be.visible");
    cy.get("#selectProcessingTypesContent").should("contain", "Processing");
  });

  it('Batch Processing button is clickable', () => {
    cy.get("#selectProcessingTypesContent .homeBtn").contains("Batch Processing")
      .should("be.visible")
      .should("not.be.disabled");
  });

  it('Single Image Processing button is clickable', () => {
    cy.get("#selectProcessingTypesContent .homeBtn").contains("Single Image Processing")
      .should("be.visible")
      .should("not.be.disabled");
  });

  it('Batch Processing shows import screen', () => {
    cy.get("#selectProcessingTypesContent .homeBtn").contains("Batch Processing").click();
    
    // Should show import interface
    cy.contains("Import Images").should("be.visible");
    cy.get(".custom-dropzone").should("be.visible");
  });

  it('Single Image Processing shows import screen', () => {
    cy.get("#selectProcessingTypesContent .homeBtn").contains("Single Image Processing").click();
    
    // Should show import interface
    cy.contains("Import Images").should("be.visible");
    cy.get(".custom-dropzone").should("be.visible");
  });

  it('Dropzone displays expected message', () => {
    cy.get("#selectProcessingTypesContent .homeBtn").contains("Batch Processing").click();
    cy.get(".custom-dropzone").should("contain", "Drag and Drop");
  });

  it('Can navigate back from processing workflow', () => {
    cy.get("#selectProcessingTypesContent .homeBtn").contains("Batch Processing").click();
    cy.wait(500);
    
    // Go back home - Home button is first button in ctlBtns
    cy.get(".ctlBtns button").first().click();
    cy.wait(500);
    // Home modal opens - home content should be visible inside modal
    cy.get("#homeContent", { timeout: 5000 }).should("be.visible");
    cy.get(".homeBtn", { timeout: 5000 }).contains("Process").should("be.visible");
  });

  it('Settings accessible during processing workflow', () => {
    cy.get("#selectProcessingTypesContent .homeBtn").contains("Batch Processing").click();
    cy.wait(500);
    
    // Open settings - Settings button is second button in ctlBtns (index 1)
    cy.get(".ctlBtns button").eq(1).click();
    cy.get(".modal-container .settings", { timeout: 5000 }).should("be.visible");
    
    // Close settings
    cy.get(".modal-container .close-button", { timeout: 5000 }).click();
    cy.wait(300);
    cy.get(".modal-container").should("not.exist");
    
    // Should still be in workflow
    cy.contains("Import Images", { timeout: 5000 }).should("be.visible");
  });
});


describe('Process Workflow', () => {
  beforeEach(() => {
    cy.visit('http://localhost:3000/');
    cy.get("#homeContent .homeBtn").contains("Process").click();
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
    cy.wait(200);
    
    // Go back home
    cy.get(".ctlBtns .feather-home").click();
    cy.get(".homeBtn").contains("Process").should("be.visible");
  });

  it('Settings accessible during processing workflow', () => {
    cy.get("#selectProcessingTypesContent .homeBtn").contains("Batch Processing").click();
    
    // Open settings
    cy.get(".ctlBtns .feather-settings").click();
    cy.get(".settings").should("be.visible");
    
    // Close settings
    cy.get(".closeDia").click();
    
    // Should still be in workflow
    cy.contains("Import Images").should("be.visible");
  });
});


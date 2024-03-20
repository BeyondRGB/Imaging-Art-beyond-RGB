describe('Select Processing Type and Import Images Tab', () => {
    beforeEach(() => {
      cy.visit('http://localhost:3000/');
      cy.get(".homeContent .homeBtn").contains("Process").click();
    })

    it('Batch Processing button works', () => {
      cy.get("#selectProcessingTypes .homeBtn").contains("Batch Processing").click();
      
      cy.get("right").get(".custom-dropzone").contains("Drag and Drop Files Here");
      cy.get("left").contains("Import Images");


    })
    it('Single Image Processing button works', () => {
      cy.get("#selectProcessingTypes .homeBtn").contains("Single Image Processing").click();

      cy.get("right").get(".custom-dropzone").contains("Drag and Drop Files Here");
      cy.get("left").contains("Import Images");
      

    })    
  })
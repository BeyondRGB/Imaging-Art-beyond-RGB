describe("Settings", () => {
	beforeEach(() => {
		cy.clearLocalStorage();
		cy.visit("/");
		cy.contains("#homeContent button", "Process").click();
		cy.get('[data-cy="nav-settings"]').click();
	});

	it("switches between dark and light theme preferences", () => {
		cy.contains("button.theme-option", "Dark").as("darkbtn");
		cy.get("@darkbtn").click();
		cy.get("html").should("have.class", "dark");
		cy.window().then(win => {
			const saved = JSON.parse(win.localStorage.getItem("appSettings") || "{}");
			expect(saved.themePreference).to.equal("dark");
		});

		cy.contains("button.theme-option", "Light").click();
		cy.get("html").should("not.have.class", "dark");
		cy.window().then(win => {
			const saved = JSON.parse(win.localStorage.getItem("appSettings") || "{}");
			expect(saved.themePreference).to.equal("light");
		});
	});

	it("toggles side navigation preference", () => {
		cy.get('[role="switch"][aria-label="Toggle side menu navigation"]').click();

		cy.get(".sideMain").should("not.exist");
		cy.window().then(win => {
			const saved = JSON.parse(win.localStorage.getItem("appSettings") || "{}");
			expect(saved.sideNav).to.equal(false);
		});
	});
});

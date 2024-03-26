# Imaging-Art-beyond-RGB

## Beyond RGB Frontend
![Electron - 15.3.0](https://badgen.net/badge/Electron/v15.3.0/cyan)  ![Svelte - 3.44.2](https://badgen.net/badge/Svelte/v3.44.2/orange) ![Typescript - 4.5.3](https://badgen.net/badge/Typescript/v4.5.3/blue) ![Tailwindcss - 3.0.2](https://badgen.net/badge/Tailwindcss/v3.0.2/purple) ![Vite - 2.7.1](https://badgen.net/badge/Vite/v2.7.1/green) ![Electron Forge - 6.0.0-beta.61](https://badgen.net/badge/Electron%20Forge/v6.0.0-beta.61/red)

[**Live Demo**](https://beyondrgb.d1lrrwy1z5590.amplifyapp.com/) *(Works best on Chrome)*

  

**Docs:**
>  -  [Electron](https://www.electronjs.org/docs/latest/) (Desktop) - [Video (1min 48sec)](https://www.youtube.com/watch?v=m3OjWNFREJo)
>
>  -  [Svelte](https://svelte.dev/docs) (Display) - [Video (2min 11sec)](https://www.youtube.com/watch?v=rv3Yq-B8qp4)
>
>  -  [Typescript](https://www.typescriptlang.org/docs/) (Code) - [Video (2min 24sec)](https://www.youtube.com/watch?v=zQnBQ4tB3ZA)
>
>  -  [Tailwindcss](https://tailwindcss.com/docs) (Style) - [Video (2min 20sec)](https://www.youtube.com/watch?v=mr15Xzb1Ook)
>
>  -  [Vite](https://vitejs.dev/guide/) (Compile/Dev) - [Video (9min 55sec)](https://www.youtube.com/watch?v=5IG4UmULyoA)
>
>  -  [Electron Forge](https://www.electronforge.io/) (Build) - [Video (11min 11sec)](https://www.youtube.com/watch?v=3yqDxhR2XxE)

  

# Usage

  

## Install Dependencies

  

`npm install` 

  >downloads and installs node_modules 

## Starting Development

  

`npm run dev` 

>starts development environment by starting vite's dev server and electron app in dev mode

  

## Preview Application

  

`npm run preview`

>builds svelte for production and starts electron app in dev mode

  

## Building Executable

  

`npm run build`  

>builds svelte and electron for production

  

## Building Distributable (Installer)

  

`npm run make`  

>builds svelte and electron for production and packages into an installer



## Install and Run Cypress

1) First, navigate to the "/frontend" folder

2) Install and run Cypress using the commands below

`npm install`  

>installs Cypress locally from packages in package.json and package-lock.json

`npm run dev`  

>runs the frontend locally (application must be running in order to run Cypress tests)

`npx cypress open`  

>opens Cypress

3) Once the Cypress UI pops up, choose "E2E Testing"

4) Choose Electron for the browser

5) Navigate to "Specs" and choose the test file that you wish to run

  

# Troubleshooting

There have been reports that using the Git Bash command line on Windows specifically with this template will prevent the Electron app from rendering (packaged apps are fine). We recommend that on Windows, you use CMD.exe, PowerShell, or WSL.

  

---

  

BetterThanReallyGoodBread - R.I.T.
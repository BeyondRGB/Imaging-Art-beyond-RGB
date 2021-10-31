## Beyond-RGB-Frontend

Electron + Svelte + Tailwindcss + Vite + Electron Forge

[Live Demo](https://dev.d2m0gyo8ptlels.amplifyapp.com/) (Works best on Chrome)

Docs: 
> - [Electron - v15.3.0](https://www.electronjs.org/docs/latest/) (Desktop)
    - [Video (1min 48sec)](https://www.youtube.com/watch?v=m3OjWNFREJo)
> - [Svelte - v3](https://svelte.dev/docs) (Display)
    - [Video (2min 11sec)](https://www.youtube.com/watch?v=rv3Yq-B8qp4)
> - [Tailwindcss - v2](https://tailwindcss.com/docs) (Style)
    - [Video (2min 20sec)](https://www.youtube.com/watch?v=mr15Xzb1Ook)
> - [Vite - v2](https://vitejs.dev/guide/) (Bundler/Dev Server)
    - [Video (9min 55sec)](https://www.youtube.com/watch?v=5IG4UmULyoA)
> - [Electron Forge - v6](https://www.electronforge.io/) (Build)
    - [Video (11min 11sec)](https://www.youtube.com/watch?v=3yqDxhR2XxE)

# Usage

## Install Dependencies

`yarn` *or npm install*

## Starting Development

`yarn dev` *or npm run dev*
starts development environment by starting vite's dev server and electron app in dev mode

## Preview Application

`yarn preview` *or npm run preview*
builds svelte for production and starts electron app in dev mode

## Building Executable

`yarn build` *or npm run build*
builds svelte and electron for production

## Building Distributable (Installer)

`yarn make` *or npm run make*
builds svelte and electron for production and packages into an installer

# Troubleshooting
There have been reports that using the Git Bash command line on Windows specifically with this template will prevent the Electron app from rendering (packaged apps are fine). We recommend that on Windows, you use CMD.exe, PowerShell, or WSL.

---

BetterThanReallyGoodBread - R.I.T.
import React, {useState, useEffect} from 'react';
import _ from 'lodash';
import {
  Switch,
  Route,
  HashRouter
} from "react-router-dom";
import { VscDebugConsole, VscFlame, VscRadioTower, VscRuby, VscTerminalDebian } from "react-icons/vsc";

import SideMenu from './components/sideMenu';
import TopNav from './components/topNav';
import MenuIcon from './components/menuIcon';

import LaunchPage from './pages/launchPage'
import SpectralOverlay from './pages/spectralOverlay';
import ManagedRGB from './pages/managedRGB';

import './styles/App.css';
const routes = [
  {
    path: "/managed-rgb",
    exact: true,
    title: 'Color Managed RGB Image',
    menu: () => [
      {text: 'Color Managed RGB Image', isSelected: true, url: '/managed-rgb', icon: <VscRadioTower size="32"/>},
      {text: 'Spectral Overlay', url: '/spec-overlay', icon: <VscFlame size="32"/>},
      {text: 'Home', url: '/', icon: <VscDebugConsole size="32"/>},
      {text: 'Settings', url: '/', icon: <VscRuby size="32"/>},
    ],
    main: () => <ManagedRGB />
  },
  {
    path: "/spec-overlay",
    exact: true,
    title: 'Spectral Overlay',
    menu: () => [
      {text: 'Color Managed RGB Image', url: '/managed-rgb', icon: <VscRadioTower size="32"/>},
      {text: 'Spectral Overlay', isSelected: true, url: '/spec-overlay', icon: <VscFlame size="32"/>},
      {text: 'Home', url: '/', icon: <VscDebugConsole size="32"/>},
      {text: 'Settings', url: '/', icon: <VscRuby size="32"/>},

    ],
    main: () => <SpectralOverlay />
  },
  {
    path: "/",
    exact: false,
    title: 'Beyond RGB',
    menu: () => [
      {text: 'Home', url: '/', isSelected: true, icon: <VscDebugConsole size="32"/>},
      {text: 'Settings', url: '/', icon: <VscRuby size="32"/>},
    ],
    main: () => <LaunchPage />
  },
]

function App(): JSX.Element {
  return (
    <HashRouter>
      <div className="App">

        <TopNav>
          <Switch>
            {routes.map((route, index) => 
              <Route key={index} path={route.path} exact={route.exact} children={route.title}/>
            )}
          </Switch>
        </TopNav>

        <SideMenu logo={{text: 'Beyond RGB', icon: <VscTerminalDebian size="48"/>}}>
          <Switch>
            {routes.map((route, index) => 
              <Route
                key={index}
                path={route.path}
                exact={route.exact}
                children={route.menu().map((value: any, index: any) =>
                  <MenuIcon key={index} text={value.text} 
                    url={value.url} icon={value.icon} 
                    isSelected={value.isSelected}/>
                )}
              />
            )}
          </Switch>
        </SideMenu>

        <Switch>
          {routes.map((route, index) => 
            <Route key={index} path={route.path} exact={route.exact} children={<route.main />}/>
          )}
        </Switch>
      </div>
    </HashRouter>
  )
}

export default App;
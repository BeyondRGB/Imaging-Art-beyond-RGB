import React, {useState, useEffect} from 'react';
import _ from 'lodash';
import './styles/App.css'

function App(): JSX.Element {
  return (
    <div className="App">
      {StartPage()}
    </div>
  )
}

function StartPage(): JSX.Element {
  return (
    <div className="StartPage">
      <div className="top-nav">
        Nav
      </div>
      <div className="side-menu">
        Menu
        {_.range(10).map((value, index) => 
          <li>{index}</li>
        )}
      </div>
      <div className="start-buttons">
        Buttons
        <button>Process</button>
        <button>View</button>
      </div>
    </div>
  )
}

export default App;
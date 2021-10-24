import React from 'react';

export default function SideMenu(props: any): JSX.Element {
  return (
    <div className="side-menu group">
      <div className="logo">
        <span className="logo-text">
          {props.logo.text}
        </span>
        {props.logo.icon}
      </div>
      <div className="menu-items">
        {props.children}
      </div>
    </div>
  )
}
import React from 'react';
import { NavLink } from 'react-router-dom';


export default function MenuIcon({ icon, text, url, isSelected = false }: any): JSX.Element {
  return (
    <NavLink to={url} className={`menu-icon ${isSelected && 'border-red-600 border-2'}`}>
      <div className="icon">
        {icon}
      </div>
      <span className="icon-text">
        {text}
      </span>
    </NavLink>
  )
}
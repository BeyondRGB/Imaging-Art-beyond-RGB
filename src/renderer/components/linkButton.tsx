import React from 'react';
import { NavLink } from 'react-router-dom';


export default function LinkButton({linkTo, className, text}: any): JSX.Element {
  return (
    <NavLink to={linkTo} className={className}>{text}</NavLink>
  )
}
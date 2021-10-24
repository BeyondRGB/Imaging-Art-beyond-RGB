import React from 'react';

export default function TopNav(props: any): JSX.Element {
  return (
    <div className="top-nav">
      {props.children}
    </div>
  )
}
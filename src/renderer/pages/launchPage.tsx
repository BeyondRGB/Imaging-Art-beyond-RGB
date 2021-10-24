import React from 'react';
import LinkButton from '../components/linkButton';

export default function LaunchPage(): JSX.Element {
    return (
      <div className="LaunchPage">
          <LinkButton text="LaunchPage" linkTo="/managed-rgb" className="start-button"/>
      </div>
    )
  }
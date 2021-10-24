import React from 'react';
import LinkButton from '../components/linkButton';

export default function ProcessPage(): JSX.Element {
    return (
      <div className="LaunchPage">
        <LinkButton text="ProcessPage" linkTo="/" className="start-button"/>
      </div>
    )
  }
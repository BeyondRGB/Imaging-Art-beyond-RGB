import React from 'react';
import LinkButton from '../components/linkButton';

export default function PreprocessPage(): JSX.Element {
    return (
      <div className="LaunchPage">
        <LinkButton text="PreprocessPage" linkTo="/" className="start-button"/>
      </div>
    )
  }
import React, { Component } from 'react';

class InputForm extends Component {
  constructor(props) {
    super(props);

    let example = this.props.example == null ? "" : this.props.example;

    this.state = {
      example: example,
    }
  }

  /* No active function since we changed the input */
  handleInputChange(value) {
    this.setState({
      example: value,
    });

    this.props.handleInputChange(value);
  }

  render() {
    let example = this.props.example

    // null means don't change value
    if(example == null) {
      example = this.state.example;
    }

    return (
      <form className="col-sm-8 termi border">
        <input 
          type="text"
          className="form-control text-white bg-dark card full"
          id="input"
          value={example}
          onChange={e => this.handleInputChange(e.target.value)}/>
      </form>
    );
  }
}

export default InputForm;

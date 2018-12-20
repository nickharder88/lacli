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
  onInputChange(value) {
    this.setState({
      example: value,
    });

    this.props.onChange();
  }

  render() {
    let example = this.state.example;

    // null means don't change value
    if(this.props.example != null) {
      example = this.props.example
    }

    return (
      <form className="col-sm-8 termi border">
        <input 
          type="text"
          className="form-control text-white bg-dark card full"
          id="input"
          value={example}
          onChange={e => this.onInputChange(e.target.value)}/>
      </form>
    );
  }
}

export default InputForm;

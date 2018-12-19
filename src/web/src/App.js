import React, { Component } from 'react';
import './App.css';
import funcsRaw from './funcs.json';
import FuncButton from './FuncButton';

class App extends Component {
  constructor(props) {
    super(props);

    let funcs = {};
    let order = [];

    for(let i = 0; i < funcsRaw.length; i++) {
      let func = funcsRaw[i];
      let key = func.name;

      order.push(key);
      funcs[key] = func;
    }

    this.state = {
      activeKey: null,
    };

    this.funcs = funcs;
    this.order = order;
  }

  setActiveFunc(key) {
    this.setState({activeKey: key});
  }

  renderFuncRows() {
    let funcRows = [];

    for(let i = 0; i < this.order.length; i++) {
      // Guarentee the same order
      let key = this.order[i];
      let func = this.funcs[key];

      let active = false;

      if(func.name === this.state.activeKey) {
        active = true;
      }

      funcRows.push(<FuncButton 
        key={key}
        func={func}
        active={active}
        onClick={this.setActiveFunc.bind(this)}/>);
    }

    return funcRows;
  }

  render() {
    let example = null;
    if(this.state.activeKey) {
      let func = this.funcs[this.state.activeKey];
      console.log(this.state.activeKey);
      console.log(func);
      example = func.example;
    }

    return (
      <div className="App container">
        <div className="row">
          <div className="col">
            <h1>Linear Algebra CLI</h1>
          </div>
        </div>
        <div className="row">
          <div className="col">
            <p>{example}</p>
          </div>
        </div>
        <div className="row">
          <div className="col list-group">
            {this.renderFuncRows()}
          </div>
        </div>
      </div>
    );
  }
}

export default App;

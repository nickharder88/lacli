import React, { Component } from 'react';
import './App.css';

import FuncRow from './FuncRow';
import ExampleRow from './ExampleRow';
import InputForm from './InputForm';

import funcsRaw from './funcs.json';
import examplesRaw from './examples.json';

class App extends Component {
  constructor(props) {
    super(props);

    let funcs = {};
    let fOrder = [];

    for(let i = 0; i < funcsRaw.length; i++) {
      let func = funcsRaw[i];
      let key = func.name;

      fOrder.push(key);
      funcs[key] = func;
    }

    this.funcs = funcs;
    this.fOrder = fOrder;

    let examples = {};
    let exOrder = [];

    for(let i = 0; i < examplesRaw.length; i++) {
      let example = examplesRaw[i];
      let key = example.id;

      exOrder.push(key);
      examples[key] = example;
    }

    this.examples = examples;
    this.exOrder = exOrder;

    // activeKey = null, setExample = false  => don't change output
    // activeKey = null, setExample = true   => change output to ""
    // activeKey = !null, setExample = false => don't change ouput
    // activeKey = !null, setExample = true  => change output to example in activeKey
    this.state = {
      activeKey: null,
      setExample: false,
      example: null,
    };
  }

  setActiveItem(key) {
    if(key == null) {
      this.setState({
        activeKey: null,
        setExample: false,
      });
      return;
    }

    // Key not found
    if(!(key in this.funcs) && !(key in this.examples)) {
      this.setState({
        activeKey: null,
        setExample: false,
      });
      return;
    }

    this.setState({
      activeKey: key,
      setExample: true,
      example: null,
    });
  }

  handleInputChange(value) {
    this.setState({
      activeKey: null,
      setExample: true,
      example: value,
    });
  }

  handleExampleChange(key, example) {
    if(key !== this.state.activeKey) {
      return;
    }

    this.setState({
      activeKey: key,
      setExample: true,
      example: example,
    });
  }

  // Renders similar styled rows for functions and examples
  renderItemRows(ItemTag, items, order) {
    let rows = [];

    for(let i = 0; i < order.length; i++) {
      // Guarentee the same order
      let key = order[i];
      let item = items[key];

      let active = false;

      if(key === this.state.activeKey) {
        active = true;
      }

      rows.push(<ItemTag
        key={key}
        item={item}
        active={active}
        onClick={this.setActiveItem.bind(this)}
        onExampleChange={this.handleExampleChange.bind(this)}/>);
    }

    return rows;
  }

  render() {
    let output = null;

    return (
      <div className="App container">
        <div className="row">
          <div className="col">
            <h1>Linear Algebra CLI</h1>
          </div>
        </div>

        {/* Terminal */}
        <div className="row">
          <InputForm handleInputChange={this.handleInputChange.bind(this)} example={this.state.example}/>
        </div>
        <div className="row">
          <div className="col-sm-8 card termo bg-dark">
            <p className="text-white">{output}</p>
          </div>
        </div>

        {/* Accordion */}
        <div className="row">
          <div className="col">
            <div id="accordion">
              {/* Basic Information */}
              <div className="card">
                <div className="card-header" id="headingBasicInfo">
                  <h5 className="mb-0">
                    <button
                      className="btn btn-link"
                      data-toggle="collapse"
                      aria-expanded="true"
                      aria-controls="collapseBasicInfo">
                      Basic Information
                    </button>
                  </h5>
                </div>

                <div
                  id="collapseBasicInfo"
                  className="collapse"
                  aria-labelledby="headingBasicInfo"
                  data-parent="#accordion">

                  <div className="card-body">
                  </div>
                </div>
              </div>

              {/* Functions */}
              <div className="card">
                <div className="card-header" id="headingFunc">
                  <h5 className="mb-0">
                    <button 
                      className="btn btn-link"
                      data-toggle="collapse"
                      data-target="#collapseFunc"
                      aria-expanded="true"
                      aria-controls="collapseFunc">
                      Functions
                    </button>
                  </h5>
                </div>

                <div
                  id="collapseFunc"
                  className="collapse"
                  aria-labelledby="headingFunc"
                  data-parent="#accordion">

                  <div className="list-group">
                    {this.renderItemRows(FuncRow, this.funcs, this.fOrder)}
                  </div>
                </div>
              </div>

              {/* Examples */}
              <div className="card">
                <div className="card-header" id="headingExample">
                  <h5 className="mb-0">
                    <button
                      className="btn btn-link"
                      data-toggle="collapse"
                      data-target="#collapseExample"
                      aria-expanded="true"
                      aria-controls="collapseExample"
                    >
                      Examples
                    </button>
                  </h5>
                </div>

                <div
                  id="collapseExample"
                  className="collapse"
                  aria-labelledby="headingExample"
                  data-parent="#accordion">

                  <div className="list-group">
                    <div className="list-group">
                      {this.renderItemRows(ExampleRow, this.examples, this.exOrder)}
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

export default App;

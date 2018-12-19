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
      example: null,
    };

    this.funcs = funcs;
    this.order = order;
  }

  setActiveFunc(key) {
    if(key == null) {
      this.setState({
        activeKey: null,
        example: null,
      });
      return;
    }

    let func = this.funcs[key];
    if(func == null) {
      console.log("Error: tried to get invalid function name");
      this.setState({
        activeKey: null,
        example: null,
      });
      return;
    }

    this.setState({
      activeKey: key,
      example: func.example,
    });
  }

  /* No active function since we changed the input */
  onInputChange(value) {
    this.setState({
      activeKey: null,
      example: value,
    });
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
          <form className="col-sm-8">
            <input 
              type="text"
              class="form-control text-white bg-dark termi card"
              id="input"
              value={this.state.example}
              onChange={e => this.onInputChange(e.target.value)}/>
          </form>
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
              <div class="card">
                <div class="card-header" id="headingBasicInfo">
                  <h5 class="mb-0">
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

                  <div class="card-body">
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
                    {this.renderFuncRows()}
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

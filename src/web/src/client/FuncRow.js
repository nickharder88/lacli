import React, { Component } from 'react';

class FuncRow extends Component {

  
  componentDidUpdate(prevProps, prevState, snapshot) {
    // just started
    if(prevProps.active === false && this.props.active === true) {
      this.props.onExampleChange(this.props.item.name, this.props.item.example);
    }
  }

  render() {
    let className = this.props.active ? "btn btn-primary active" : "btn btn-primary";

    return (
      <div className="list-group-item card">
        <div className="card-body container-fluid">
          <div className="row text-left">
            <div className="col-sm-3">
              <h4>{this.props.item.name}</h4>
            </div>
            <div className="col-sm-6">
              <p className="card-text">{this.props.item.description}</p>
            </div>
            <div className="col-sm-3 d-flex justify-content-end">
              <button 
                type="button" 
                className={className}
                onClick={() => this.props.onClick(this.props.item.name)}>
                Example
              </button>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

export default FuncRow;

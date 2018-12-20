import React, { Component } from 'react';

class FuncRow extends Component {
  render() {
    let className = this.props.active ? "btn btn-primary active" : "btn btn-primary";

    return (
      <div className="list-group-item card">
        <div className="card-body container-fluid">
          <div className="row text-left">
            <div className="col-sm-3">
              <h4>{this.props.func.name}</h4>
            </div>
            <div className="col-sm-6">
              <p className="card-text">{this.props.func.description}</p>
            </div>
            <div className="col-sm-3 d-flex justify-content-end">
              <button 
                type="button" 
                className={className}
                onClick={() => this.props.onClick(this.props.func.name)}>
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

import React, { Component } from 'react';

class FuncRow extends Component {
  render() {
    let className = this.props.active ? "btn btn-primary active" : "btn btn-primary";

    return (
      <div className="list-group-item card">
        <div className="card-body container-fluid">
          <div className="row text-left">
            <div className="col-sm-9">
              <p className="card-text">{this.props.example.description}</p>
            </div>
            <div className="col-sm-3 d-flex justify-content-end">
              <button 
                type="button" 
                className={className}
                onClick={() => this.props.onClick(this.props.example.id)}>
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

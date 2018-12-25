import React, { Component } from 'react';

class ExampleRow extends Component {

  async setExample() {
    this.props.onExampleChange(this.props.item.id, this.props.item.example[0]);

    // wait 1 second
    let promise = new Promise((resolve, reject) => {
      setTimeout(() => resolve(), 2000);
    });
    await promise;

    this.props.onExampleChange(this.props.item.id, "worked");
  }

  componentDidUpdate(prevProps, prevState, snapshot) {
    // just started
    if(prevProps.active === false && this.props.active === true) {
      this.setExample();
    }
  }

  render() {
    let className = this.props.active ? "btn btn-primary active" : "btn btn-primary";

    return (
      <div className="list-group-item card">
        <div className="card-body container-fluid">
          <div className="row text-left">
            <div className="col-sm-9">
              <p className="card-text">{this.props.item.description}</p>
            </div>
            <div className="col-sm-3 d-flex justify-content-end">
              <button 
                type="button" 
                className={className}
                onClick={() => this.props.onClick(this.props.item.id)}>
                Example
              </button>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

export default ExampleRow;

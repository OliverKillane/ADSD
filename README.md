# ELEC70046 - Advanced Digital Systems Design Helpers & Tools
## What is this?
A collection of tools I built to use in implementing the SVM coursework for 
ELEC70046. Seemed a waste to throw them away. I hope they are useful.

No actual hardware implementation for the coursework is included, because then it 
wouldn't be any fun! 😂

```bash
├── cordic   # Jupyter notebook for cordic implementation
├── impls    # example implementations for memory transfer and software SVM
|
├── docs     # FAQ + setup docs
├── misc     # miscellaneous setup files
|
├── data     # Data for training
├── models   # trained weights for the svm model
|
├── report   # a template for the report
|
└── tools    # tools for header generation
```

## The Coursework Problem
To optimise an embedded system performing SVM classification of digits from MNIST.

This is in the form of a radial basis function kernel classifier:
$$y = sgn\left( \sum_{i=1}^{N_{SV}} y_ia_i\exp\left(-\gamma \left\| x_i - x \right\|^2\right) + b\right)$$
- $\gamma = 0.001$ (scaling parameter for classifier).
- $N_{SV} = 165$ (the number of support vectors).
- $y_ia_i : \mathbb{R}^{1 \times 165}$ the product of support vector labels and weights.
- $b$ is the bias
- $x_i \in \mathbb{R}^{1 \times (165 \times 784)}$ the featurs of the support vectors.
- $x$ is a vector of $2601$ images of size $28 \times 28$ pixels
- $\exp\left(-\gamma \left\| x_i - x \right\|^2\right)$ is the kernel function
- $y \in \mathbb{R}^{2601}$ is the predictor for each image

Weight generation can be found in [training](./training/) and the results in [models](./models).


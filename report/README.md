## Latex Template
A couple of diagrams and some formatting setup.

Recommended setup is to use vscode with:
- [latex workshop](vscode:extension/James-Yu.latex-workshop)
- [drawio integration](vscode:extension/hediet.vscode-drawio)

## Mark Scheme
### Working Version (40%)
| Mark | Description |
|-|-|
| $70\%+$ | Optimised architecture for the problem |
| $60\%$   | Functionally correct working version |
| $50\%$   | Simulation only of IP but correct functionality for all inputs |
| $40\%$   | Simulation for a single input |
| $0\%$    | No correct simulation of IP |

### CORDIC optimisation (20%)
| Mark | Description |
|-|-|
| $70\%+$ | Correct impl. and use of appropriate pragmas |
| $60\%$ | Correct impl. and use of pragmas that do not have impact |
| $50\%$ | Simulation only |

### ARM Core Utilisation (10%)
| Mark | Description |
|-|-|
| $100\%$ | If ARM core is properly considered for utilisation (sending, pre-processing data) |

### Data Transfer Optimisation (10%)
| Mark | Description |
|-|-|
| $70\%+$ | Use of all available bus width and double buffer |
| $50\%$ | Use of available bus width |
| $0\%$ | Transfer Axis-light and 8 bits (expected) |

### Other Optimisations (20%)
| Mark | Description |
|-|-|
| $100\%$ | DMA, use multiple cores, word length |
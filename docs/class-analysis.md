# Explanations
This document shows both the purpose, and the current level of certainty regarding implementation of the classes

## Confidence Legend
| Level | Behavior Confidence | Name Confidence |
| :--- | :--- | :--- |
| **High** | Proven via logic, patterns, or API calls. | Verified via strings or common library naming. |
| **Medium** | Inferred from structure or context. | Plausible based on typical VST architecture. |
| **Low** | Preliminary guess / speculative. | Placeholder or likely incorrect. |

### GDIDrawingContext
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### Frame
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### HorizontalSlider
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### RotaryControl
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### VerticalSlider
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### TwoAxisSlider
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### View
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### VstPluginHelper
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### Bitmap
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### Control
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### DelayLama
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### Knob
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### DelayLamaEditor
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### SplashScreen
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### TileGrid
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### AEffGUIEditor
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### DropTarget
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### Monk
Behavior Confidence: Medium

Name Confidence: High

##### Assumptions:
- None

##### Observations:
- Constructor receives a Bitmap pointer/reference
- The used bitmap originates from DelayLamaEditor open function
- The passed bitmap is resource id 131
- Class is only instantiated from DelayLamaEditor::Open()
- No references found outside DelayLamaEditor

##### Inferences:
- Responsible for rendering the monk sprite
- Manages animation frames from sprite sheet
- Handles state updates based on user input

### AudioEffectX
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None

### AudioEffect
Behavior Confidence: None

Name Confidence: None

##### Assumptions:
- None

##### Observations:
- None

##### Inferences:
- None
## Image Viewer

Here's a brief story about this image viewer project, which primarily focuses on raw BGR888 frames. Deep learning methods have significantly impacted the computer vision industry, leading to the establishment of two primary workflows for computer vision tasks:

1. Training models for specific tasks
2. Deploying models on designated devices

The research team trained the model, while the deployment task was handled by the product team. Upon delivering the product to our customer, we discovered a significant loss in performance. After extensive investigation, we found that the discrepancy between training and inference was due to the image decoder. The research team utilized OpenCV as the image decoder, while the product team opted for libjpeg-turbo to achieve optimal product performance. As a result, there were slight pixel jitters between the decoded frames produced by the two decoders.

After identifying the root cause, we proposed two principles to improve the product performance:



1. The model should tolerate pixel jitter.
2. If human eyes cannot discern the difference, the model should perform similarly. **The image-viewer project can be utilized to display raw BGR888 frames, helping to distinguish these differences.**

### Build

For Mac users:

```
brew install sdl2 sdl2_image opencv
```



Notes: The BGR888 raw frame does not provide information regarding the image width and height.


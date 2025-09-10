    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_in_color");//holt die position der uniform variable im shader programm
if(colorUniformLocation != -1) {
    glUniform4f(colorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
}
else {
    cout << "uniform color not found" << endl;
}
int textureUniformLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_in_texture"));
if(textureUniformLocation != -1) {
    GLCALL(glUniform1d(textureUniformLocation, 0));
}
else {
    cout << "uniform texture not found" << endl;
}
int modelMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model");//holt die position der uniform variable im shader programm
if(modelMatrixLocation != -1) {
    GLCALL(glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]));//überträgt die matrix an die uniform variable im shader programm
}
else {
    cout << "uniform matrix not found" << endl;
}
int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");//holt die position der uniform variable im shader programm
if(colorUniformLocation != -1) {
    GLCALL(glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelViewProj[0][0]));
}
else {
    cout << "uniform modelviewproj not found" << endl;
}
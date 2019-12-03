
void display(){

}
/* Allow the user to quit with 'esc'
 * Preconditions: glutKeyboardFunc(keyboard) must be called
 * Postconditions: The user can exit the program by pressing the 'esc' key
*/
void keyboard(unsigned char key, int x, int y){
  switch(key){
    case 27:
      exit(0);
      break;
  }
}

void main(){
  glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Terrain");

  glutDisplayFunc(display);


  glutMainLoop();
}

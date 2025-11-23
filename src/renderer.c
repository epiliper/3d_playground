typedef struct {
  unsigned int vao, vbo, shader;
} RenderInfo;

// An interface that describes an object that can render itself.
typedef struct {
  RenderInfo rinfo;
  void (*render)(void *rinfo, void *aux);
} Renderable;

typedef struct {}

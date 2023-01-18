#include "SceneView.h"

namespace OM3D {

SceneView::SceneView(const Scene* scene) : _scene(scene) {
}

Camera& SceneView::camera() {
    return _camera;
}

const Camera& SceneView::camera() const {
    return _camera;
}

void SceneView::render() const {
    if(_scene) {
        _scene->render(_camera);
    }
}

void SceneView::render_transparent(Texture &head_list) const {
    if(_scene) {
        _scene->render_transparent(_camera, head_list);
    }
}

void SceneView::point_lights_render(std::shared_ptr<StaticMesh> sphere_mesh) const 
{
    if (_scene) 
    {
        _scene->point_lights_render(_camera, sphere_mesh);
    }
}

void SceneView::deferred_render() const {
    if(_scene) {
        _scene->deferred_render(_camera);
    }
}

}

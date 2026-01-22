#ifndef COMPOSIT_COMPONENT_H
#define COMPOSIT_COMPONENT_H

#include <image_processing/ImgProcessingComponent.h>

class CompositComponent : public ImgProcessingComponent {
  public:
    CompositComponent(std::string name) : ImgProcessingComponent(name) {}
    jsoncons::json get_component_list() override {
        jsoncons::json body;
        body.insert_or_assign("name", this->get_name());
        jsoncons::json component_list = jsoncons::json::make_array();
        for (auto &component : this->components) {
            component_list.push_back(component->get_component_list());
        }
        body.insert_or_assign("component", component_list);
        return body;
    }

    void
    init_components(const std::vector<std::shared_ptr<ImgProcessingComponent>>
                        &components) {
        for (auto &component : components) {
            this->components.push_back(component);
        }
    }

  protected:
    std::vector<std::shared_ptr<ImgProcessingComponent>> components;
};

#endif // COMPOSIT_COMPONENT_H
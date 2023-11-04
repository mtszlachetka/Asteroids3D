#include "Body.hpp"
#include "System.hpp"
#include "ModelLoader.hpp"



System createSol() {
    renderContext sphereContext = ModelLoader::load("../models/sphere.obj");
    Body Sun{sphereContext, glm::vec3(0.9, 0.9, 0.2), 5.f};
    Body Mercury { sphereContext, glm::vec3(0.8, 0.8, 0.8), 0.4};
    Body Earth{sphereContext, glm::vec3(0., 0.1, 0.9), 1.f};
    Body Moon{sphereContext, glm::vec3(0.7, 0.7, 0.7), 0.3f};
    Body Venus{sphereContext, glm::vec3(0.5, 0.5, 0.), 0.9f};
    Body Mars{sphereContext, glm::vec3(0.9, 0., 0.), 0.3f};
    Body Deimos {sphereContext, glm::vec3(0.9f, 0.9f, 0.9f), 0.1f};
    Body Phobos {sphereContext, glm::vec3(0.9f, 0.9f, 0.9f), 0.05f};
    Body Jupiter {sphereContext, glm::vec3(0.5, 0.2, 0.2), 11.f};

    System MoonSystem(Moon, { 0., 1.6, 0. }, 30.f);
    System EarthSystem(Earth, { 0., 30.5, 0. }, 365.f);
    System VenusSystem(Venus, { 0., -20.5, 0.}, 225.f);
    System SolSystem(Sun, glm::vec3(0.f), 0.f);
    System MarsSystem(Mars, {0., -40., 0.}, 687.f);
    System MerucrySystem(Mercury, { 0., 10., 0.}, 88.f);
    System DeimosSystem(Deimos, {0., 0.5, 0.}, 20.f);
    System PhobosSystem(Phobos, {0., -0.4, 0.}, 20.f);
    System JupiterSystem(Jupiter, {0., 150., 0.}, 12 * 365.f);

    EarthSystem.attachSystem(MoonSystem);
    MarsSystem.attachSystem(DeimosSystem);
    MarsSystem.attachSystem(PhobosSystem);
    SolSystem.attachSystem(EarthSystem);
    SolSystem.attachSystem(VenusSystem);
    SolSystem.attachSystem(MarsSystem);
    SolSystem.attachSystem(MerucrySystem);
    SolSystem.attachSystem(JupiterSystem);

    // double system
    Body Dummy {sphereContext, glm::vec3(0.), 0.0001};
    Body Dwarf {sphereContext, glm::vec3(0.4, 0.1, 0.1), 0.3};

    System DummySystem(Dummy, {0., -80., 0.}, 1000.f);
    System Planet1S(Dwarf, {0., 0., 1.}, 20.f);
    System Planet2S(Dwarf, {0., 0., -1.}, 20.f);
    System Planet3S(Dwarf, {0, -std::sqrt(3) * 40., 40}, 1000.f);
    System Planet4S(Dwarf, {0, std::sqrt(3) * 40., -40}, 1000.f);

    DummySystem.attachSystem(Planet1S);
    DummySystem.attachSystem(Planet2S);
    SolSystem.attachSystem(Planet3S);
    SolSystem.attachSystem(Planet4S);
    SolSystem.attachSystem(DummySystem);

    return SolSystem;
}
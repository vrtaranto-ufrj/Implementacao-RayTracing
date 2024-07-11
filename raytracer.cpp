#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

// Estrutura para representar um vetor 3D
struct Vec3 {
    double x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3 &v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    Vec3 operator-(const Vec3 &v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    Vec3 operator*(const Vec3 &v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }

    Vec3 operator*(double t) const {
        return Vec3(x * t, y * t, z * t);
    }

    Vec3 operator/(double t) const {
        return Vec3(x / t, y / t, z / t);
    }

    double dot(const Vec3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vec3 normalize() const {
        double mag = std::sqrt(x * x + y * y + z * z);
        return Vec3(x / mag, y / mag, z / mag);
    }
};

// Operador para multiplicar um escalar por um vetor
Vec3 operator*(double t, const Vec3 &v) {
    return Vec3(t * v.x, t * v.y, t * v.z);
}

// Estrutura para representar um raio
struct Ray {
    Vec3 origin, direction;

    Ray(const Vec3 &origin, const Vec3 &direction) : origin(origin), direction(direction.normalize()) {}
};

// Função para verificar interseção com uma esfera
bool hit_sphere(const Vec3 &center, double radius, const Ray &ray, double &t) {
    Vec3 oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false;
    } else {
        t = (-b - sqrt(discriminant)) / (2.0 * a);
        return true;
    }
}

// Função para calcular a cor de um raio com iluminação de Phong
Vec3 ray_color(const Ray &ray, const Vec3 &light_pos, int depth) {
    Vec3 sphere_center1(-1, 0, -1); // Esfera esquerda (vermelha)
    Vec3 sphere_center2(1, 0, -1);  // Esfera direita (verde)

    if (depth <= 0) {
        return Vec3(0, 0, 0); // Cor preta para profundidade zero
    }

    double t;
    Vec3 hit_point, N, reflected_direction;
    Vec3 light_intensity(1.5, 1.5, 1.5); // Intensidade da luz aumentada
    double ambient = 0.1; // Componente ambiental

    if (hit_sphere(sphere_center1, 0.5, ray, t)) {
        hit_point = ray.origin + ray.direction * t;
        N = (hit_point - sphere_center1).normalize();
        reflected_direction = ray.direction - 2 * ray.direction.dot(N) * N;
        Ray reflected_ray(hit_point, reflected_direction);
        
        Vec3 light_dir = (light_pos - hit_point).normalize();
        double diff = std::max(0.0, N.dot(light_dir));
        Vec3 diff_color = Vec3(1, 0, 0) * diff; // Cor difusa vermelha
        Vec3 ambient_color = Vec3(1, 0, 0) * ambient; // Cor ambiental vermelha
        return 0.5 * (ambient_color + diff_color) * light_intensity;
    }

    if (hit_sphere(sphere_center2, 0.5, ray, t)) {
        hit_point = ray.origin + ray.direction * t;
        N = (hit_point - sphere_center2).normalize();
        reflected_direction = ray.direction - 2 * ray.direction.dot(N) * N;
        Ray reflected_ray(hit_point, reflected_direction);
        
        Vec3 light_dir = (light_pos - hit_point).normalize();
        double diff = std::max(0.0, N.dot(light_dir));
        Vec3 diff_color = Vec3(0, 1, 0) * diff; // Cor difusa verde
        Vec3 ambient_color = Vec3(0, 1, 0) * ambient; // Cor ambiental verde
        return 0.5 * (ambient_color + diff_color) * light_intensity;
    }

    return Vec3(0.0, 0.0, 0.0); // Fundo preto
}

int main() {
    const int image_width = 800;
    const int image_height = 400;
    const int max_depth = 5; // Profundidade máxima de recursão

    // Posição da luz
    Vec3 light_pos(1, 0, 0); // Um pouco para a direita da câmera

    // Criação da imagem usando OpenCV
    cv::Mat image(image_height, image_width, CV_8UC3);

    Vec3 origin(0, 0, 0);
    Vec3 horizontal(4, 0, 0);
    Vec3 vertical(0, 2, 0);
    Vec3 lower_left_corner(-2, -1, -1);

    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            double u = double(i) / (image_width - 1);
            double v = double(j) / (image_height - 1);
            Ray r(origin, lower_left_corner + horizontal * u + vertical * v - origin);
            Vec3 color = ray_color(r, light_pos, max_depth);
            cv::Vec3b &pixel = image.at<cv::Vec3b>(image_height - j - 1, i);
            pixel[0] = static_cast<int>(255.999 * color.z); // Blue
            pixel[1] = static_cast<int>(255.999 * color.y); // Green
            pixel[2] = static_cast<int>(255.999 * color.x); // Red
        }
        // Exibir a imagem a cada linha processada
        cv::imshow("Ray Tracing", image);
        cv::waitKey(1); // Espera 1ms para atualizar a janela
    }

    cv::imwrite("output.png", image);
    cv::waitKey(0); // Espera até que uma tecla seja pressionada

    return 0;
}

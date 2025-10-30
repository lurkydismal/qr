FROM archlinux:latest

RUN pacman-key --init && pacman-key --populate archlinux

# Enable multilib
# RUN sed -i '/\[multilib\]/,/Include/s/^#//' /etc/pacman.conf

# Add multilib
RUN echo -e "\n[multilib]\nInclude = /etc/pacman.d/mirrorlist" >> /etc/pacman.conf

RUN pacman -Syu --noconfirm

RUN pacman -S --noconfirm --needed \
    binutils \
    clang mold \
    lib32-glibc

RUN rm -rf /var/cache/pacman/pkg/* /tmp/*

WORKDIR /build

COPY . .

RUN ./build_release.sh

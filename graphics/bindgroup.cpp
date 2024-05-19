#include "bindgroup.h"

namespace sgd {

namespace {

uint32_t nextHash(uint32_t bindGroupIndex) {
	static uint32_t hashes[4];
	SGD_ASSERT(bindGroupIndex < 4);
	return hashes[bindGroupIndex]++;
}

} // namespace

BindGroupDescriptor::BindGroupDescriptor(
	const char* label,												 //
	uint32_t bindGroupIndex,										 //
	Vector<wgpu::BindGroupLayoutEntry> bindGroupLayoutEntries,		 //
	Vector<wgpu::VertexBufferLayout> vertexBufferLayouts,			 // Only for geometry shaders right now...
	String shaderSource)											 //
	: label(label), wgpuBindGroupIndex(bindGroupIndex),				 //
	  wgpuBindGroupLayoutEntries(std::move(bindGroupLayoutEntries)), //
	  wgpuVertexBufferLayouts(std::move(vertexBufferLayouts)),		 //
	  wgpuShaderSource(std::move(shaderSource)),					 //
	  hash(nextHash(bindGroupIndex)) {
}

wgpu::BindGroupLayout BindGroupDescriptor::wgpuBindGroupLayout(GraphicsContext* gc) const {

	using Cache = Map<const BindGroupDescriptor*, wgpu::BindGroupLayout>;

	static Cache cache;

	auto& layout = cache[this];
	if (layout) return layout;

	wgpu::BindGroupLayoutDescriptor desc{};
	desc.entryCount = wgpuBindGroupLayoutEntries.size();
	desc.entries = wgpuBindGroupLayoutEntries.data();
	desc.label = label;

	return layout = gc->wgpuDevice().CreateBindGroupLayout(&desc);
}

BindGroup::BindGroup(const BindGroupDescriptor* desc) : m_desc(desc), m_resources(m_desc->wgpuBindGroupLayoutEntries.size()) {
}

void BindGroup::setResource(uint32_t index, CGraphicsResource* resource) {
	if (m_resources[index] == resource) return;

	removeDependency(m_resources[index]);
	addDependency(m_resources[index] = resource);

	invalidate(true);
}

void BindGroup::setBuffer(uint32_t index, CBuffer* buffer) {
	SGD_ASSERT(index < m_desc->wgpuBindGroupLayoutEntries.size());
	SGD_ASSERT(m_desc->wgpuBindGroupLayoutEntries[index].buffer.type != wgpu::BufferBindingType::Undefined);

	setResource(index, buffer);
}

CBuffer* BindGroup::getBuffer(uint32_t index) const {
	SGD_ASSERT(index < m_desc->wgpuBindGroupLayoutEntries.size());
	SGD_ASSERT(m_desc->wgpuBindGroupLayoutEntries[index].buffer.type != wgpu::BufferBindingType::Undefined);

	return m_resources[index] ? m_resources[index]->as<const Buffer>() : nullptr;
}

void BindGroup::setTexture(uint32_t index, CTexture* texture) {
	SGD_ASSERT(index < m_desc->wgpuBindGroupLayoutEntries.size());
	SGD_ASSERT(m_desc->wgpuBindGroupLayoutEntries[index].texture.sampleType != wgpu::TextureSampleType::Undefined);

	setResource(index, texture);
	if (index + 1 < m_desc->wgpuBindGroupLayoutEntries.size() &&
		m_desc->wgpuBindGroupLayoutEntries[index + 1].sampler.type != wgpu::SamplerBindingType::Undefined) {
		setResource(index + 1, texture);
	}
}

CTexture* BindGroup::getTexture(uint32_t index) const {
	SGD_ASSERT(index < m_desc->wgpuBindGroupLayoutEntries.size());
	SGD_ASSERT(m_desc->wgpuBindGroupLayoutEntries[index].texture.sampleType != wgpu::TextureSampleType::Undefined);

	return m_resources[index] ? m_resources[index]->as<const Texture>() : nullptr;
}

void BindGroup::onValidate(GraphicsContext* gc) const {
	auto& bglEntries = m_desc->wgpuBindGroupLayoutEntries;

	Vector<wgpu::BindGroupEntry> entries(bglEntries.size());

	for (int i = 0; i < entries.size(); ++i) {
		entries[i].binding = i;
		if (bglEntries[i].buffer.type != wgpu::BufferBindingType::Undefined) {
			entries[i].buffer = m_resources[i]->as<const Buffer>()->wgpuBuffer();
		} else if (bglEntries[i].texture.sampleType != wgpu::TextureSampleType::Undefined) {
			entries[i].textureView = m_resources[i]->as<const Texture>()->wgpuTextureView();
		} else if (bglEntries[i].sampler.type != wgpu::SamplerBindingType::Undefined) {
			entries[i].sampler = m_resources[i]->as<const Texture>()->wgpuSampler();
		} else {
			SGD_ABORT();
		}
	}

	wgpu::BindGroupDescriptor desc{};
	desc.layout = m_desc->wgpuBindGroupLayout(gc);
	desc.entryCount = entries.size();
	desc.entries = entries.data();
	desc.label = m_desc->label;
	m_bindGroup = gc->wgpuDevice().CreateBindGroup(&desc);
}

BindGroup* emptyBindGroup(uint32_t index) {

	static BindGroupPtr bindGroups[3];

	if (bindGroups[index]) return bindGroups[index];

	static const BindGroupDescriptor descs[]{{"emptyBindGroup0", 0, {}, {}, {}}, //
											 {"emptyBindGroup1", 1, {}, {}, {}}, //
											 {"emptyBindGroup2", 2, {}, {}, {}}};

	bindGroups[index] = new BindGroup(&descs[index]);

	return bindGroups[index];
}

} // namespace sgd
